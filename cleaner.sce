// This file is released under the 3-clause BSD license. See COPYING-BSD.
// Generated by builder.sce : Please, do not edit this file
// cleaner.sce
// ------------------------------------------------------
curdir = pwd();
cleaner_path = get_file_path('cleaner.sce');
chdir(cleaner_path);
// ------------------------------------------------------
if fileinfo('loader.sce') <> [] then
  mdelete('loader.sce');
end
// ------------------------------------------------------
if fileinfo('libnetwork.so') <> [] then
  mdelete('libnetwork.so');
end
// ------------------------------------------------------
if fileinfo('libnetwork.c') <> [] then
  mdelete('libnetwork.c');
end
// ------------------------------------------------------
chdir(curdir);
// ------------------------------------------------------
