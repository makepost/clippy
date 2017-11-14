project('clippy','c',
  version: '0.1.0',
  meson_version: '>= 0.40.0',
)

gnome = import('gnome')

prefix = get_option('prefix')

libdir = join_paths(prefix, get_option('libdir'))
datadir = join_paths(prefix, get_option('datadir'))

pkglibdir = join_paths(libdir, meson.project_name())
pkgdatadir = join_paths(datadir, meson.project_name())

girdir = join_paths(pkgdatadir, 'gir-1.0')
typelibdir = join_paths(pkglibdir, 'girepository-1.0')

gjs_console = find_program('gjs')

girepository = dependency('gobject-introspection-1.0')
gjs = dependency('gjs-1.0')
gtk3 = dependency('gtk+-3.0', version: '>= 3.21.6')

config_h = configuration_data()
config_h.set_quoted('GETTEXT_PACKAGE', 'clippy')
configure_file(
  output: 'clippy-config.h',
  configuration: config_h,
)
add_project_arguments([
  '-I' + meson.build_root(),
], language: 'c')

subdir('src')
