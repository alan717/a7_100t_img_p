using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
//using System.Reflection;
using System.Collections;
//using System.Linq;
//using UnityEditor;
using UnityEngine;
using Mono.Cecil;
using Mono.Cecil.Cil;
using System.Diagnostics.Contracts;
//using Mono.Cecil.Inject;

//using CustomCodeAttributes;

namespace cstester
{// targets, from assemblies to parameters.
 //
 
     
 
    class Program
    {
        private const string paths = "D:\\QQSPEED\\fix_qqspeed.dll";




        public static  void test()
        {
            System.Console.WriteLine(Environment.StackTrace);

        }
        static void Main(string[] args)
        {
            //test();
            String[] line = { };
            var readerParameters = new ReaderParameters { ReadSymbols = false };

            var resolver = new DefaultAssemblyResolver();
            resolver.AddSearchDirectory(@"D:\etc\a7_100t_img_p\csharp\cstester\cstester");
            AssemblyDefinition assembly = AssemblyDefinition.ReadAssembly("D:\\QQSPEED\\qqspeed.dll",
                new ReaderParameters { AssemblyResolver = resolver });// ;
            //AssemblyDefinition assembly=AssemblyDefinition.ReadAssembly("D:\\QQSPEED\\qqspeed.dll", readerParameters);// ;
            System.IO.StreamWriter file =new System.IO.StreamWriter(@"D:\QQSPEED\fix.txt");
            foreach (TypeDefinition type in assembly.MainModule.Types)
            {
                //Writes the full name of a type
                //Console.WriteLine("name:{0},", type.FullName);
                //System.IO.File.WriteAllText(@"D:\QQSPEED\NEW.TXT", type.FullName);
                //file.WriteLine(type.FullName);
            }
            try
            {
                var module = assembly.MainModule;
                foreach (Mono.Cecil.TypeDefinition type in module.Types)
                {
                    Console.WriteLine("name:{0},", type.FullName);
                    file.WriteLine("module.name:{0}",type.FullName);
                    foreach (MethodDefinition method in type.Methods)
                    {
                        Console.WriteLine("name:{0},", method.Name);
                        file.WriteLine("method.name:{0}",method.FullName);
                        //if (method.IsConstructor || method.IsGetter || method.IsSetter || !method.IsPublic)
                        //    continue;
                        //InjectMethod(module, method); 
                        if (method.FullName == "System.Void NssLogServer::SendMsg(Protocol.CSMsg)")
                        {
                               InjectModule(module,method);
                            // InjectMethod(method,assembly);
                            // Instruction ins = method.Body.Instructions[0];
                        }

                    }
                }
                assembly.Write(paths, new WriterParameters { WriteSymbols = false });
            

            }catch(Exception ex)
            {
                System.Console.WriteLine(ex.Message);
                System.Console.WriteLine(ex.StackTrace);
                System.Console.WriteLine(Environment.StackTrace);
            }
             
            ////assembly.
            //TypeDefinition bar = assembly.MainModule.GetType("Bar");
            //MethodDefinition foo = bar.GetMethod("Foo");                             // Using the extension method as if it were native
            //MethodDefinition foo2 = TypeDefinitionExtensions.GetMethod(bar, "Foo");
            //System.Console.Write("ok");
        }
        /// &lt;summary&gt;
        /// ���ǰ����Instruction, �����ص�ǰ���
        /// &lt;/summary&gt;
        private static Instruction InsertBefore(ILProcessor worker, Instruction target, Instruction instruction)
        {
            worker.InsertBefore(target, instruction);
            return instruction;
        }

        /// &lt;summary&gt;
        /// �������Instruction, �����ص�ǰ���
        /// &lt;/summary&gt;
        private static Instruction InsertAfter(ILProcessor worker, Instruction target, Instruction instruction)
        {
            worker.InsertAfter(target, instruction);
            return instruction;
        }
        //����ע���ĺ���ƫ��ֵ
        private static void ComputeOffsets(MethodBody body)
        {
            var offset = 0;
            foreach (var instruction in body.Instructions)
            {
                instruction.Offset = offset;
                offset += instruction.GetSize();
            }
        }
        private static void InjectMethod(MethodDefinition method, AssemblyDefinition assembly)
        {
            var firstIns = method.Body.Instructions.First();
            var worker = method.Body.GetILProcessor();

            //��ȡDebug.Log��������
            var hasPatchRef = assembly.MainModule.Import(typeof(Debug).GetMethod("Log", new Type[] { typeof(string) }));
            //���뺯��
            var current = InsertBefore(worker, firstIns, worker.Create(OpCodes.Ldstr, "Inject"));
            current = InsertBefore(worker, firstIns, worker.Create(OpCodes.Call, hasPatchRef));
            //����Offset
            ComputeOffsets(method.Body);



        }

        private static void InjectModule(ModuleDefinition module, MethodDefinition method)
        {

            // �����Ժ���õ�����
            var objType = module.ImportReference(typeof(System.Object));
            var intType = module.ImportReference(typeof(System.Int32));
            var logFormatMethod =
                module.ImportReference(typeof(Debug).GetMethod("Log", new[] { typeof(string)}));

            // ��ʼע��IL����
            var insertPoint = method.Body.Instructions[0];

            var ilProcessor = method.Body.GetILProcessor();


            //var body = new MethodBody(method);
            //var insertPoint = body.Instructions[0];
            //var ilProcessor = body.GetILProcessor();

            //ilProcessor.InsertAfter()
            ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Ldstr, "Inject"));
            ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Call, logFormatMethod));
            // ����һЩ��ǩ���������ת
            //var label1 = ilProcessor.Create(OpCodes.Ldarg_1);
            //var label2 = ilProcessor.Create(OpCodes.Stloc_0);
            //var label3 = ilProcessor.Create(OpCodes.Ldloc_0);
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Nop));
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Ldstr, "a = {0}, b = {1}"));
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Ldc_I4_2));
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Newarr, objType));
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Dup));
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Ldc_I4_0));
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Ldarg_0));
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Box, intType));
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Stelem_Ref));
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Dup));
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Ldc_I4_1));
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Ldarg_1));
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Box, intType));
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Stelem_Ref));
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Call, logFormatMethod));
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Ldarg_0));
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Ldarg_1));
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Ble, label1));
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Ldarg_0));
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Br, label2));
            //ilProcessor.InsertBefore(insertPoint, label1);
            //ilProcessor.InsertBefore(insertPoint, label2);
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Br, label3));
            //ilProcessor.InsertBefore(insertPoint, label3);
            //ilProcessor.InsertBefore(insertPoint, ilProcessor.Create(OpCodes.Ret));
        }

 
         

        public override string ToString()
        {
            return base.ToString();
        }

        public override bool Equals(object obj)
        {
            return base.Equals(obj);
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }
    }
}
