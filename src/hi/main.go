package main

import "fmt"

func main(){
    fmt.Println("You're in main")
    say_hi()
    fmt.Println("Bye!")
}

func say_hi() {
    fmt.Println("Hello from Go 8=")
}
