import shutil, platform, subprocess
import commands
import re,os

_dirname = './.config.tmp'
_sub = re.compile(r'[\.-\/]')
_id = platform.system()
_cl_options = '/ZI /nologo /W3 /WX- /Od /Oy- /D "DEBUG" /D "NOMINMAX" /D "_MBCS" /D "PLATFORM_WIN32" /Gm /EHsc /RTC1 /MTd /GS /fp:precise /Zc:wchar_t /Zc:forScope /Gd /analyze- /errorReport:queue '
class ConfigBuilder :

    def __init__(self, config_list, path_to_config_h) :
        self._config_list = config_list
        self._success_list = []
        self._path_to_config_h = path_to_config_h
        if not os.path.isdir(_dirname):
            os.makedirs(_dirname)
        
    def Build(self) :
        config_h_path = self._path_to_config_h + '/config.h'
        if not os.path.isfile(config_h_path) :
            for config in self._config_list :
                print 'checking for ' + config + '...',
                head = '#include <' + config + '>\n'
                head += '''int main() {
                         return 0;
                       }\n'''
                value = re.sub(_sub, '_', config).upper()
                filename = _dirname + '/' + value.lower() + '.cc'
                test_file = open(filename, 'w+')
                test_file.write(head)
                test_file.close()
                if self._RunChecking(filename, value.lower()) == 0 :
                    self._success_list.append('HAVE_' + value)
                    print ' yes'
                else :
                    print ' no'
            print 'now making config header...'
            config_h = open(config_h_path, 'w+')
            for ma in self._success_list :
                config_h.write('#define ' + ma + '\n\n')
            config_h.close()
            print 'success'
        return self._success_list

    def _RunChecking(self, name, obj) :
        if _id == 'Windows' or id == 'Microsoft':
            os.system('@echo off')
            ret = subprocess.call('cl.exe ' + _cl_options + name, stdout=subprocess.PIPE)
            os.system('@echo on')
            if ret == 0 :
                os.system('del /Q ' + obj + '.*')
            elif ret != 0 and os.path.isfile(obj + '.obj') :
                ret = 0
                os.system('del /Q ' + obj + '.*')
            return ret
        else :
            return commands.getstatusoutput('g++ ' + name)[0]
            
