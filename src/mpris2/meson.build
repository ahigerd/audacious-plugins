gnome = import('gnome')


gio_dep = dependency('gio-2.0', version: glib_req, required: true)
aud_dbus_deps = [gio_dep]


if not have_windows
  gio_unix_dep = dependency('gio-unix-2.0', version: glib_req, required: true)
  aud_dbus_deps += [gio_unix_dep]
endif


mpris2_core_src = gnome.gdbus_codegen('object-core',
  sources: 'mpris2.xml',
  interface_prefix: 'org.mpris.',
  namespace: 'Mpris'
)


mpris2_player_src = gnome.gdbus_codegen('object-player',
  sources: 'mpris2-player.xml',
  interface_prefix: 'org.mpris.',
  namespace: 'Mpris'
)


have_mpris2 = true


shared_module('mpris2',
  mpris2_core_src,
  mpris2_player_src,
  'plugin.cc',
  dependencies: [audacious_dep, glib_dep] + aud_dbus_deps,
  name_prefix: '',
  install: true,
  install_dir: general_plugin_dir
)
