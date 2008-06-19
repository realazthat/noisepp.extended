package.name = "toimage"
package.kind = "exe"
package.language = "c++"
package.config["Debug"].bindir = "../bin/Debug"
package.config["Release"].bindir = "../bin/Release"
package.objdir = "obj/examples/toimage"
if options["target"] then
	package.path = "build/"..options["target"]
end
package.files = { "../../examples/toimage/main.cpp" }
package.includepaths = { "../../noisepp/core", "../../noisepp/utils", "../../noisepp/threadpp" }
package.links = { "noisepp" }
if (target == "gnu") then
  package.linkoptions = { "-pthread" }
end
