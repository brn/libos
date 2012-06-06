{
  'target_defaults': {
    'conditions': [
      ['OS != "win"', {
        'defines': [
          '_LARGEFILE_SOURCE',
          '_FILE_OFFSET_BITS=64',
          '_GNU_SOURCE'
        ],
        'conditions': [
          ['OS=="solaris"', {
            'cflags': ['-pthreads'],
            'ldlags': ['-pthreads'],
          }, {
            'cflags': ['-pthread'],
            'ldlags': ['-pthread'],
          }],
        ],
      }],
    ],
  },

  'targets': [
    {
      'target_name': 'os',
      'type': '<(library)',
      'include_dirs': [
        'include',
        'src/',
        '<(additional_include)'
      ],
      'direct_dependent_settings': {
        'include_dirs': [ 'include' ],
        'conditions': [
          ['OS=="linux"', {
            'libraries': [ '-ldl' ],
          }],
        ],
      },
      'defines': [
        'HAVE_CONFIG_H'
      ],
      'sources': [
        'common.gypi',
        'include/thread.h',
        'include/utilities.h',
        'include/fs.h',
        'include/notificator/notificator.h',
        'include/notificator/notifcator-impl.h',
        'include/notificator/listener_adapter.h',
        'include/notificator/listener_adapter-impl.h',
        'include/logging.h',
        'include/smart_pointer/common/ptr_deleter.h',
        'include/smart_pointer/common/ptr_handle-impl.h',
        'include/smart_pointer/common/ptr_handle.h',
        'include/smart_pointer/common/ptr_handle_base.h',
        'include/smart_pointer/common/ptr_traits.h',
        'include/smart_pointer/refcount.h',
        'include/smart_pointer/scoped_list-impl.h',
        'include/smart_pointer/scoped_ptr-impl.h',
        'include/smart_pointer/shared_ptr-impl.h',
        'include/smart_pointer/refcount-impl.h',
        'include/smart_pointer/refcount_base.h',
        'include/smart_pointer/scoped_list.h',
        'include/smart_pointer/scoped_ptr.h',
        'include/smart_pointer/shared_ptr.h',
        'src/lib/unordered_map.h',
        'src/fs/directory/directory-inl.h',
        'src/fs/path/path.cc',
        'src/fs/mv/mv.cc',
        'src/fs/rm/rm.cc',
        'src/fs/mkdir/mkdir.cc',
        'src/fs/stat/stat.cc',
        'src/fs/event/event.cc',
        'src/fs/event/md5.h',
        'src/fs/event/md5.cc',
        'src/utils/memory/pool.cc',
        'src/utils/memory/pool-inl.h',
        'src/utils/utils.h',
        'src/utils/log/logging.cc'
      ],
      'conditions': [
        [ 'OS=="win"', {
          'sources': [
            'src/utils/utils-win32.cc',
            'src/thread/thread-win32.cc',
            'src/fs/directory/directory-win32.cc',
            'src/fs/event/win32/fs_watcher_win32.cc'
          ]
        }, { # Not Windows i.e. POSIX
          'cflags': [
            '-g',
            '--std=gnu89',
            '-pedantic',
            '-Wall',
            '-Wextra',
            '-Wno-unused-parameter'
          ],
          'sources': [
            'src/utils/utils-posix.cc',
            'src/thread/thread-posix.cc',
            'src/fs/directory/directory-posix.cc'
          ]
        }],
        [ 'OS=="mac"', {
          'sources': [ 
            'src/fs/event/macos/fs_watcher_macos.cc'
          ],
          'direct_dependent_settings': {
            'libraries': [
              '$(SDKROOT)/System/Library/Frameworks/CoreServices.framework',
            ],
          }
        }],
        [ 'OS=="linux"', {
          'sources': [
            'src/fs/event/linux/fs_watcher_linux.cc'
          ]
        }]
      ]
    }
  ]
}


