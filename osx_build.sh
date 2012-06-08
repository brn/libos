if [ "${1}" = "clean" ] ; then
    xcodebuild -configuration Release clean
    xcodebuild -configuration Debug clean
elif [ "${2}" = "64" ] ; then
    xcodebuild -project os.xcodeproj -configuration ${1} -target All ARCHS=x86_64
else
    xcodebuild -project os.xcodeproj -configuration ${1} -target All
fi
    
