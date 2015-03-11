gateway_path = get_absolute_file_path("builder.sce");
libname = "servergateway"
namelist = [
"server", "sci_server"
];
files = [
"saved.c"
];
ldflags = "";
cflags = "-g";
libs = [];
tbx_build_gateway(libname, namelist, files, gateway_path, ..
 libs, ldflags, cflags);
clear tbx_build_gateway;
