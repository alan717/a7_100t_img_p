package main

import (
    "fmt"
    "time"
)

func main() {
    requests := make(chan int, 5)
    for i := 1; i <= 2; i++ {
        requests <- i
    }
    close(requests)
    limiter := time.Tick(time.Millisecond * 200)

    for req := range requests { //会循环两次，前面往requests channel中发送了两个值
        <-limiter  //执行到这里，需要隔 200毫秒才继续往下执行，time.Tick(timer)上面已定义
        fmt.Println("request", req, time.Now())
    }

    burstyLimiter := make(chan time.Time, 3)

    for i := 0; i < 3; i++ {
        burstyLimiter <- time.Now()  //这里burstyLimiter channel 连续发送了三次值
    }

    go func() {
        for t := range time.Tick(time.Second * 2) {
            burstyLimiter <- t // 这里往burstyLimiter channel 循环发送time.Tick
        }
    }()
    burstyRequests := make(chan int, 5)

    for i := 1; i <= 5; i++ {
        burstyRequests <- i
        <-burstyLimiter  //前三次没有速度限制，会直接打印出后面的println的内容
        fmt.Println("request", i, time.Now())
    }

    close(burstyRequests)
    for req := range burstyRequests {
        <-burstyLimiter  //继续接收burstyLimiter值，除了前三次，后面的都是time.Tick进行速度限制,2秒打印一次，直到此次循环结束
        fmt.Println("request", req, time.Now())
    }
}