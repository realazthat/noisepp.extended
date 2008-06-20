project.name = "noisepp"
project.configs = { "Release", "Debug" }
if options["target"] then
	project.path = "build/"..options["target"]
end

dopackage("noisepp")
dopackage("examples/toimage")
dopackage("examples/test")

function domakeall(cmd, arg)
    os.execute("premake --usetargetpath --target vs2002")
    os.execute("premake --usetargetpath --target vs2003")
    os.execute("premake --usetargetpath --target vs2005")
    os.execute("premake --usetargetpath --target vs2008")
    os.execute("premake --usetargetpath --target gnu")
end