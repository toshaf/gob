#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>
#include <map>
#include <string>
#include <functional>

#include "gob_internal.hh"

using std::map;
using std::string;
using std::function;

void gob_trace(string path, map<gob::Address, gob::Symbol> symbols, function<void(gob::Symbol)> handler) {

    int pid = fork();
    if (pid < 0)
        throw gob::gob_exception("Couldn't start process with " + path);

    if (pid == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);

        char *argv[] = { const_cast<char*>(path.c_str()), NULL };
        char *envp[] = { NULL };

        execve(const_cast<char*>(path.c_str()), argv, envp);
    }

    int status = 0;

    wait(&status);

    if (!WIFSTOPPED(status))
        throw gob::gob_exception("Couldn't trace target process");

    map<int, long int> bps;
    for (auto i = symbols.begin(); i != symbols.end(); ++i) {
        // store the existing byte
        auto value = ptrace(PTRACE_PEEKTEXT, pid, i->first, 0);
        bps[i->first] = value;

        // and set a breakpoint
        ptrace(PTRACE_POKETEXT, pid, i->first, 0xCC);
    }

    ptrace(PTRACE_CONT, pid, 0, 0);

    while (1) {
        wait(&status);

        if (WIFEXITED(status)) {
            break;
        }

        if (WIFSTOPPED(status)) {
           struct user_regs_struct regs;
           ptrace(PTRACE_GETREGS, pid, NULL, &regs);

           // bp has just been executed so 
           regs.rip -= 1;

           auto sym = symbols.find(regs.rip);
           if (sym != symbols.end()) {
                // replace the value
                ptrace(PTRACE_POKETEXT, pid, regs.rip, bps[regs.rip]);
                // move the ins pointer
                ptrace(PTRACE_SETREGS, pid, 0, &regs);
                // invoke the handler
                handler(sym->second);
           }

           // and then carry on with the run
           ptrace(PTRACE_CONT, pid, NULL, NULL);
        }
    }
}
