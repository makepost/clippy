src_res = gnome.compile_resources('org.gnome.Clippy.src',
  'org.gnome.Clippy.src.gresource.xml',
  gresource_bundle: true,
  install: true,
  install_dir: pkgdatadir,
)

bin_conf = configuration_data()
bin_conf.set('GJS', find_program('gjs').path())
bin_conf.set('PACKAGE_VERSION', meson.project_version())
bin_conf.set('PACKAGE_NAME', meson.project_name())
bin_conf.set('prefix', get_option('prefix'))
bin_conf.set('libdir', join_paths(get_option('prefix'), get_option('libdir')))
bin_conf.set('datadir', join_paths(get_option('prefix'), get_option('datadir')))

configure_file(
  input: 'org.gnome.Clippy.in',
  output: 'org.gnome.Clippy',
  configuration: bin_conf,
  install: true,
  install_dir: get_option('bindir')
)

libsources = [
  'lib/clippy-board.c',
  'lib/clippy-board.h'
]

libargs = [
  '-DG_LOG_USE_STRUCTURED',
  '-DG_LOG_DOMAIN="Clippy"'
]
libclippy = shared_library('clippy-1.0', libsources,
  dependencies: [gtk3],
  c_args: libargs,
  install: true,
  install_dir: pkglibdir
)

gnome.generate_gir(libclippy,
  sources: libsources,
  nsversion: '1.0',
  namespace: 'Clippy',
  symbol_prefix: 'clippy',
  identifier_prefix: 'Clippy',
  includes: ['Gtk-3.0'],
  extra_args: '--quiet',
  install_dir_gir: girdir,
  install_dir_typelib: typelibdir,
  install: true
)

