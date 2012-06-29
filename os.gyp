{
  'target_defaults': {
    'conditions': [
      ['OS != "win"', {
        'defines': [
          '_LARGEFILE_SOURCE',
          '_FILE_OFFSET_BITS=64',
          '_GNU_SOURCE',
          'BOOST_NO_TYPEID'
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
        'include/lib/sparsehash-2.0.2/src/'
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
        'include/notificator/notificator-impl.h',
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
        'include/lib/unordered_map.h',
        'src/worker/worker.cc',
        'src/worker/worker_queue.cc',
        'src/worker/worker_count.cc',
        'src/worker/channel.cc',
        'src/fs/directory/directory.cc',
        'src/fs/path/path.cc',
        'src/fs/mv/mv.cc',
        'src/fs/rm/rm.cc',
        'src/fs/mkdir/mkdir.cc',
        'src/fs/stat/stat.cc',
        'src/fs/event/event.cc',
        'src/fs/event/md5.h',
        'src/fs/event/md5.cc',
        'src/fs/io/fs_io.cc',
        'src/fs/copy/copy.cc',
        'src/utils/pool.cc',
        'src/utils/utils.h',
        'src/utils/log/logging.cc'
      ],
      'conditions': [
        [ 'OS=="win"', {
          'sources': [
            'src/utils/utils-win32.cc',
            'src/fs/event/win32/fs_watcher_win32.cc',
            'src/fs/directory/directory_defines-win32.h',
            'src/fs/directory/directory_defines-win32.cc'
          ],
          'include_dirs' : [
            '<(additional_include)'
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
            'src/fs/directory/directory_defines-posix.h',
            'src/fs/directory/directory_defines-posix.cc'
          ]
        }],
        [ 'OS=="mac"', {
          'sources': [ 
            'src/fs/event/macos/fs_watcher_macos.cc'
          ],
          'direct_dependent_settings': {
            'libraries': [
              '$(SDKROOT)/System/Library/Frameworks/CoreServices.framework'
            ],
          },
          'xcode_settings': {
            'OTHER_LDFLAGS': [
              '/opt/local/lib/<(boost_thread_lib_name)'
            ]
          }
        }],
        [ 'OS=="linux"', {
          'sources': [
            'src/fs/event/linux/fs_watcher_linux.cc'
          ]
        }]
      ]
    },
    {
      'target_name': 'run-test',
      'type': 'executable',
      'dependencies': [ 'os' ],
      'sources': [
        'test/test.cc',
      ],   
      'conditions' : [
        ['OS=="win"', {
          'include_dirs' : [
            '<(additional_include)'
          ]
        }]
      ],
      'msvs-settings': {
        'VCLinkerTool': {
          'SubSystem': 1, # /subsystem:console
        }
      }
    },
    {
      'target_name': 'fs_event_test',
      'type': 'executable',
      'dependencies': [ 'os' ],
      'include_dirs' : ['tools'],
      'defines' : ['CURRENT_DIR="<(current_dir)"', 'GTEST_HAS_RTTI=0'],
      'sources': [
        'test/fs_event_test.cc',
        'tools/gtest/gtest-all.cc',
        'tools/gtest/gtest_main.cc'
      ],
      'conditions' : [
        ['OS=="win"', {
          'include_dirs' : [
            '<(additional_include)'
          ]
        }]
      ],
      'msvs-settings': {
        'VCLinkerTool': {
          'SubSystem': 1, # /subsystem:console
        }
      }
    },
    {
      'target_name': 'notificator_test',
      'type': 'executable',
      'dependencies': [ 'os' ],
      'include_dirs' : ['tools'],
      'defines' : ['CURRENT_DIR="<(current_dir)"', 'GTEST_HAS_RTTI=0'],
      'sources': [
        'test/notificator_test.cc',
        'tools/gtest/gtest-all.cc',
        'tools/gtest/gtest_main.cc'
      ],
      'conditions' : [
        ['OS=="win"', {
          'include_dirs' : [
            '<(additional_include)'
          ]
        }]
      ],
      'msvs-settings': {
        'VCLinkerTool': {
          'SubSystem': 1, # /subsystem:console
        }
      }
    }
  ]
}


