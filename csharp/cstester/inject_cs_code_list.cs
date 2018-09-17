0	0000	ldstr	"[{0}]:{1}"
1	0005	ldstr	"xclient"
2	000A	ldarg.0
3	000B	call	class [mscorlib]System.String [mscorlib]System.String::Format(class [mscorlib]System.String, class [mscorlib]System.Object, class [mscorlib]System.Object)
4	0010	call	class [mscorlib]System.Void [UnityEngine]UnityEngine.Debug::Log(class [mscorlib]System.Object)



0	0000	nop
1	0001	call	string [mscorlib]System.Environment::get_StackTrace()
2	0006	call	void [mscorlib]System.Console::WriteLine(string)
3	000B	nop
