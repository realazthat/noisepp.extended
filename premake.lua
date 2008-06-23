project.name = "noisepp"
project.configs = { "Release", "Debug" }
if options["target"] then
	project.path = "build/"..options["target"]
end

dopackage("noisepp")
dopackage("examples/tutorial1")
dopackage("examples/tutorial2")
dopackage("examples/tutorial3")
dopackage("examples/tutorial4")
dopackage("examples/tutorial5")
dopackage("examples/tutorial6")
dopackage("examples/toimage")
dopackage("examples/test")

function domakeall(cmd, arg)
    os.execute("premake --usetargetpath --target vs2002 --os windows")
    os.execute("premake --usetargetpath --target vs2003 --os windows")
    os.execute("premake --usetargetpath --target vs2005 --os windows")
    os.execute("premake --usetargetpath --target vs2008 --os windows")
    os.execute("premake --usetargetpath --target gnu --os linux")
end
