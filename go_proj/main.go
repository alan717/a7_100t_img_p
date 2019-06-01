package main

import (
	"fmt"
	"github.com/astaxie/beego"
	"io/ioutil"
)

type MyStuct struct {
	Header string
	Body   []byte
}

func (p *MyStruct) save() error {
	filename := p.Header + ".txt"
	return ioutil.WriteFile(filename, p.Body, 0600)
}
func main() {
	//fmt.Println("Hello, World!")
	beego.Run()
}
