package.name = "noisepp"
package.kind = "lib"
package.language = "c++"
package.config["Debug"].libdir = "../lib/Debug"
package.config["Debug"].buildflags = { "optimize-speed" }
package.config["Release"].libdir = "../lib/Release"
package.config["Release"].buildflags = { "no-symbols", "optimize-speed", "no-frame-pointer" }
package.config["Release"].defines = { "NDEBUG" }
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

if (target == "vs2005") or (target == "vs2008") then
  package.config["Release"].buildoptions = { "/Ox /Ob2 /Oi /Ot /Oy /arch:SSE2 /fp:fast" }
end
