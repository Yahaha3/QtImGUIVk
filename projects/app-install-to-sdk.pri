
# 设定了目标路径，生成的插件文件自动会进入这些路径下，不必再追加INSTALLS
#CONFIG(release, debug|release): DESTDIR = $$PWD/../installer/data/plugins
#CONFIG(debug, debug|release): DESTDIR = $$PWD/../src/aos-plugins/plugins/debug

defineTest(copyApp) {
win32{
    CONFIG(debug, debug|release): {

        EXTRA_BINFILES += $$quote($$_PRO_FILE_PWD_/../src/lib_x64/win/Debug/debug/*.pdb)
        EXTRA_BINFILES += $$quote($$_PRO_FILE_PWD_/../src/lib_x64/win/Debug/debug/*.exe)

        EXTRA_BINFILES_WIN = $${EXTRA_BINFILES}
        EXTRA_BINFILES_WIN ~= s,/,\\,g

        DESTDIR_WIN = $$(AOSDK_KERNEL_ROOT)/../debug
        DESTDIR_WIN ~= s,/,\\,g

        QMAKE_POST_LINK += $$quote(if not exist $${DESTDIR_WIN} mkdir $${DESTDIR_WIN} $$escape_expand(\n\t))

        for(FILE,EXTRA_BINFILES_WIN){
            QMAKE_POST_LINK += $$quote(cmd /c copy /y $${FILE} $${DESTDIR_WIN}$$escape_expand(\n\t))
        }

#        ###### copy sdk files
#        EXTRA_SDKFILES += $$quote($$(PHUGIA_ENTITIES_ROOT)/src/lib_x64/win/Debug/$$1/debug/*.lib)

#        EXTRA_SDKFILES_WIN = $${EXTRA_SDKFILES}
#        EXTRA_SDKFILES_WIN ~= s,/,\\,g

#        DESTDIR_SDK_WIN = $$(AOSDK_KERNEL_ROOT)
#        DESTDIR_SDK_WIN ~= s,/,\\,g

#        QMAKE_POST_LINK += $$quote(if not exist $${DESTDIR_SDK_WIN} mkdir $${DESTDIR_SDK_WIN} $$escape_expand(\n\t))

#        for(FILE,EXTRA_SDKFILES_WIN){
#            QMAKE_POST_LINK += $$quote(cmd /c copy /y $${FILE} $${DESTDIR_SDK_WIN}$$escape_expand(\n\t))
#        }

        export(QMAKE_POST_LINK)
    }

    CONFIG(release, debug|release): {
        EXTRA_BINFILES += $$quote($$_PRO_FILE_PWD_/../src/lib_x64/win/Release/release/*.exe)

        EXTRA_BINFILES_WIN = $${EXTRA_BINFILES}
        EXTRA_BINFILES_WIN ~= s,/,\\,g

        DESTDIR_WIN = $$(AOSDK_KERNEL_ROOT)/../release
        DESTDIR_WIN ~= s,/,\\,g

        QMAKE_POST_LINK += $$quote(if not exist $${DESTDIR_WIN} mkdir $${DESTDIR_WIN} $$escape_expand(\n\t))

        for(FILE,EXTRA_BINFILES_WIN){
            QMAKE_POST_LINK += $$quote(cmd /c copy /y $${FILE} $${DESTDIR_WIN}$$escape_expand(\n\t))
        }

        ###### copy pdb files
        EXTRA_PDBFILES += $$quote($$_PRO_FILE_PWD_/../src/lib_x64/win/Release/release/*.pdb)

        EXTRA_PDBFILES_WIN = $${EXTRA_PDBFILES}
        EXTRA_PDBFILES_WIN ~= s,/,\\,g

        DESTDIR_PDB_WIN = $$(AOS_KERNEL_ROOT)/bin/win64/release-pdb
        DESTDIR_PDB_WIN ~= s,/,\\,g

        QMAKE_POST_LINK += $$quote(if not exist $${DESTDIR_PDB_WIN} mkdir $${DESTDIR_PDB_WIN} $$escape_expand(\n\t))

        for(FILE,EXTRA_PDBFILES_WIN){
            QMAKE_POST_LINK += $$quote(cmd /c copy /y $${FILE} $${DESTDIR_PDB_WIN}$$escape_expand(\n\t))
        }

#        ###### copy sdk files
#        EXTRA_SDKFILES += $$quote($$(PHUGIA_ENTITIES_ROOT)/src/lib_x64/win/Release/$$1/release/*.lib)

#        EXTRA_SDKFILES_WIN = $${EXTRA_SDKFILES}
#        EXTRA_SDKFILES_WIN ~= s,/,\\,g

#        DESTDIR_SDK_WIN = $$(AOSDK_KERNEL_ROOT)/
#        DESTDIR_SDK_WIN ~= s,/,\\,g

#        QMAKE_POST_LINK += $$quote(if not exist $${DESTDIR_SDK_WIN} mkdir $${DESTDIR_SDK_WIN} $$escape_expand(\n\t))

#        for(FILE,EXTRA_SDKFILES_WIN){
#            QMAKE_POST_LINK += $$quote(cmd /c copy /y $${FILE} $${DESTDIR_SDK_WIN}$$escape_expand(\n\t))
#        }

        export(QMAKE_POST_LINK)
    }
}
unix{
    CONFIG(release, debug|release):{
        APPNAME = $$quote($$_PRO_FILE_PWD_/../src/lib_x64/lnx/Release/$${TARGET})
        DESTDIR_LNX = $$(AOS_KERNEL_ROOT)/bin/lnx/release
        QMAKE_POST_LINK += $$quote(mkdir -p $${DESTDIR_LNX} $$escape_expand(\n\t))
        QMAKE_POST_LINK += $$quote(cp $${APPNAME} $${DESTDIR_LNX}$$escape_expand(\n\t))
        export(QMAKE_POST_LINK)
    }
    CONFIG(debug, debug|release):{
        APPNAME = $$quote($$_PRO_FILE_PWD_/../src/lib_x64/lnx/Debug/$${TARGET})
        DESTDIR_LNX = $$(AOS_KERNEL_ROOT)/bin/lnx/debug
        QMAKE_POST_LINK += $$quote(mkdir -p $${DESTDIR_LNX} $$escape_expand(\n\t))
        QMAKE_POST_LINK += $$quote(cp $${APPNAME} $${DESTDIR_LNX}$$escape_expand(\n\t))
        export(QMAKE_POST_LINK)
    }
}
}
