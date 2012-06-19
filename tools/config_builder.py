import shutil, platform, subprocess, sys
import commands
import re,os

_dirname = './.config.tmp'
_sub = re.compile(r'[\.\-\/\<\>]')
_id = platform.system()
_cl_options = '/ZI /nologo /W3 /WX- /Od /Oy- /D "DEBUG" /D "NOMINMAX" /D "_MBCS" /D "PLATFORM_WIN32" /Gm /EHsc /RTC1 /MTd /GS /fp:precise /Zc:wchar_t /Zc:forScope /Gd /analyze- /errorReport:queue '
_lib_path = ('/usr/local/lib', '/opt/local/lib', '/usr/lib', '/lib')
class ConfigBuilder :

    def __init__(self, path_to_config_h) :
        self._success_list = {}
        self._failed_list = {}
        self._path_to_config_h = path_to_config_h
        if not os.path.isdir(_dirname):
            os.makedirs(_dirname)

    def CheckHeader(self, must, config_list, message) :
        return self._Check(must, config_list, message)
        
    def CheckLib(self, must, config_list, message) :
        return self._Check(must, config_list, message, lib = True)

    def CheckLibAndHeader(self, must, config_list, message) :
        success = False
        for config in config_list :
            success = self.CheckHeader(must, config['header'], message)
            success = self.CheckLib(must, config['lib'], message)
        return success
    
    def CheckStruct(self, must, config_list, message) :
        self._Check(must, config_list, message, code = True)

    def Build(self) :
        config_h_path = self._path_to_config_h
        if not os.path.isfile(config_h_path) :
            print 'now making config header...'
            config_h = open(config_h_path, 'w+')
            for ma in self._success_list :
                config_h.write('#define ' + ma + '\n\n')
            for ma in self._failed_list :
                config_h.write('//#undef ' + ma + '\n\n')
            config_h.close()
            print 'success'
            return self._success_list

    def _Check(self, must, config_list, message, **cond) :
        lib = cond.has_key('lib')
        code = cond.has_key('code')
        header_list = []
        success = False
        if not os.path.isfile(self._path_to_config_h) :
            for target in config_list :
                struct = ''
                fn = ''
                is_fn = False
                if code :
                    header_list = target['header']
                    if target.has_key('struct') :
                        struct = target['struct']
                    elif target.has_key('function') :
                        fn = target['function']
                        is_fn = True
                    target = target['name']
                print 'checking for ' + target + '...',
                head = ''
                if not lib and not code :
                    head = '#include <' + target + '>\n'
                elif code :
                    for header in header_list :
                        head += '#include <' + header + '>\n'
                    if not is_fn :
                        head += 'typedef ' + struct + ' test_check;\n'
                    else :
                        head += '#include <stdio.h>\n'
                        head += '''template<typename T>\nvoid test_holder(T v) {printf("ok!\\n");};
                                '''
                        head += '''int main() {
                                     test_holder(''' + fn + ''');\n
                                   }\n'''

                if not is_fn :
                    head += '''int main() {
                         return 0;
                       }\n'''
                value = re.sub(_sub, '_', target).upper()
                value = value.replace('::', '_')
                filename = _dirname + '/' + value.lower() + '.cc'
                test_file = open(filename, 'w+')
                test_file.write(head)
                test_file.close()
                if self._RunChecking(filename, value.lower(), target, lib) == 0 :
                    if not self._success_list.has_key('HAVE_' + value) :
                        self._success_list['HAVE_' + value] = 1
                        print ' yes'
                    else :
                        break;
                    success = True
                    break
                else :
                    self._failed_list['HAVE_' + value] = 1
                    success = False
                    print ' no'
            if must and not success :
                print message
                sys.exit(1)
            return success

    def _RunChecking(self, name, obj, raw_name, lib) :
        if _id == 'Windows' or id == 'Microsoft':
            os.system('@echo off')
            if not lib :
                ret = subprocess.call('cl.exe ' + _cl_options + name, stdout=subprocess.PIPE)
            else :
                subprocess.call('cl.exe /nologo /Fo ' + name, stdout=subprocess.PIPE)
                ret = subprocess.call('link.exe /nologo "' + raw_name + '" "' + obj + '.obj"', stdout=subprocess.PIPE)
            if ret == 0 :
                os.system('del /Q ' + obj + '.*')
            elif ret != 0 and os.path.isfile(obj + '.obj') and not lib :
                ret = 0
                os.system('del /Q ' + obj + '.*')
            elif lib :
                os.system('del /Q ' + obj + '.*')
            os.system('@echo on')
            return ret
        else :
            if not lib :
                ret = commands.getstatusoutput('g++ -c ' + name)[0]
                if ret == 0 :
                    os.system('rm ' + obj + '.o')
            else :
                if raw_name.endswith('.so') :
                    ret = commands.getstatusoutput('g++ ' + name + ' -l ' + raw_name)[0]
                else :
                    ret = commands.getstatusoutput('g++ ' + raw_name + ' ' + name)[0]
                    if ret != 0 :
                        for path in _lib_path :
                            if os.path.isdir(path) :
                                ret = commands.getstatusoutput('g++ ' + path + '/' + raw_name + ' ' + name)[0]
                                if ret == 0 :
                                    break
                    if ret == 0 :
                        os.system('rm a.out')
            return ret
            
