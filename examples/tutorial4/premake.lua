package.name = "tutorial4"
package.kind = "exe"
package.language = "c++"
package.config["Debug"].bindir = "../bin/Debug"
package.config["Debug"].buildflags = { "optimize-speed" }
package.config["Release"].bindir = "../bin/Release"
package.config["Release"].buildflags = { "no-symbols", "optimize-speed", "no-frame-pointer" }
package.config["Release"].defines = { "NDEBUG" }
package.objdir = "obj/examples/tutorial4"
if options["target"] then
	package.path = "build/"..options["target"]
end
package.files = { "../../examples/tutorial4/main.cpp" }
package.includepaths = { "../../noisepp/core", "../../noisepp/utils", "../../noisepp/threadpp" }
package.links = { "noisepp" }
if (target == "gnu") then
  package.linkoptions = { "-pthread" }
  package.config["Release"].buildoptions = { "-ffast-math -funroll-loops" }
end

if (target == "vs2005") or (target == "vs2008") then
  package.config["Release"].buildoptions = { "/Ox /Ob2 /Oi /Ot /Oy /arch:SSE2 /fp:fast" }
end
