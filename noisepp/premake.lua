package.name = "noisepp"
package.kind = "lib"
package.language = "c++"
package.config["Debug"].libdir = "../lib/Debug"
package.config["Release"].libdir = "../lib/Release"
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

