package main

import (
	"fmt"
	"html/template"
	"log"
	"net/http"
)

// handler
func handler(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "Hi Here,I love %s!", r.URL.Path[1:])
}

// main
func main() {
	http.HandlerFunc("/", handler)
	log.Fatal(http.ListenAndServe(":8080", nil))
}
