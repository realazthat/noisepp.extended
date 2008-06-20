package.name = "toimage"
package.kind = "exe"
package.language = "c++"
package.config["Debug"].bindir = "../bin/Debug"
package.config["Release"].bindir = "../bin/Release"
package.config["Release"].buildflags = { "no-symbols", "optimize-speed", "no-frame-pointer" }
package.objdir = "obj/examples/toimage"
if options["target"] then
	package.path = "build/"..options["target"]
end
package.files = { "../../examples/toimage/main.cpp" }
package.includepaths = { "../../noisepp/core", "../../noisepp/utils", "../../noisepp/threadpp" }
package.links = { "noisepp" }
if (target == "gnu") then
  package.linkoptions = { "-pthread" }
  package.config["Release"].buildoptions = { "-ffast-math -funroll-loops" }
end
