using System;
using System.Reflection;

class Program
{
    static void Main(string[] args)
    {
        if (args.Length != 1)
        {
            Console.WriteLine("Usage: FindEmbeddedResourcesInPE <assembly-path>");
            return;
        }

        var assemblyPath = args[0];
        Console.WriteLine($"May Allah help me find embedded resources in the assembly [{assemblyPath}]");

        var assembly = Assembly.LoadFrom(assemblyPath);
        foreach (var name in assembly.GetManifestResourceNames())
        {
            Console.WriteLine($"Resource name: {name}");
        }
    }
}
