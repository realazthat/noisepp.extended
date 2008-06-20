package.name = "noisepp"
package.kind = "lib"
package.language = "c++"
package.config["Debug"].libdir = "../lib/Debug"
package.config["Release"].libdir = "../lib/Release"
package.config["Release"].buildflags = { "no-symbols", "optimize-speed", "no-frame-pointer" }
package.objdir = "obj/noisepp"
if options["target"] then
	package.path = "build/"..options["target"]
end
package.files = {
	matchfiles("../../noisepp/core/*.h"),
	matchfiles("../../noisepp/utils/*.cpp", "../../noisepp/utils/*.h"),
	matchfiles("../../noisepp/threadpp/*.h")
}
package.includepaths = { "../../noisepp/core", "../../noisepp/utils", "../../noisepp/threadpp" }
if (target == "gnu") then
  package.config["Release"].buildoptions = { "-ffast-math -funroll-loops" }
end
