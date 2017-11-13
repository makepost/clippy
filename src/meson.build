gnome = import('gnome')

libsources = [
  'lib/clippy-board.c',
  'lib/clippy-board.h'
]

libargs = [
  '-DG_LOG_USE_STRUCTURED',
  '-DG_LOG_DOMAIN="Clippy"'
]
libclippy = shared_library('clippy-1.0', libsources,
  dependencies: [gio, gtk3],
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
  includes: ['Gio-2.0', 'Gtk-3.0'],
  extra_args: '--quiet',
  install_dir_gir: girdir,
  install_dir_typelib: typelibdir,
  install: true
)

