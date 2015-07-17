local CppPlugin = {};

local Lib  = require("../lib");
local Path = require("../path");
local File = require("../file");

function CppPlugin:new (prj)
    local o = {
        project = function () return prj; end
    }; 
    
    self.__index = self;
    return setmetatable(o, self);
end

function CppPlugin:create ()
    local pro = self:project();
    local gbs = pro:gbs(); 
    local gbsHomeDir = gbs:homeDir();
    
    local proName = pro:name();
    local proDir  = Path.join('.gbs', proName);
    local proFile = Path.join(proDir, gbs:projectFileName());
    local proLanguage = pro:language();
    local proType = pro:type();
    local proSrcFileList = {};
    local proIncludeDirList = {};
    
    if not Path.exists('src') then
        Lib.assert(Path.mkdir('src'));
    end
    
    --
    -- Create `src' directory for any type of project
    --
    local proSrcDir = Path.join('src', proName);
    if not Path.exists(proSrcDir) then
         Lib.assert(Path.mkdir(proSrcDir));
    end
    
    --
    -- Create `include' directory if project is a library
    --
    if proType == 'shared-lib' or proType == 'static-lib' then
        if not Path.exists('include') then
            Lib.assert(Path.mkdir('include'));
        end
        table.insert(proIncludeDirList, Lib.quote('../include'))
        if proLanguage == 'C++' then
            table.insert(proSrcFileList, Lib.quote('../include/**.hpp'));
            table.insert(proSrcFileList, Lib.quote('../include/**.h'));
        elseif proLanguage == 'C' then
            table.insert(proSrcFileList, Lib.quote('../include/**.h'));
        end
    end
    
    local templateDir = nil;
    local mainSrc = nil;
    local mainDest = nil;
    
    if proLanguage == 'C++' then
        templateDir = Path.join(gbsHomeDir, 'template', 'cpp');
        mainSrc = Path.join(templateDir, proType .. '.cpp');
        mainDest = Path.join(proSrcDir, 'main.cpp');
        table.insert(proSrcFileList, Lib.quote('../../src/' .. proName .. '/**.cpp'));
--        table.insert(proSrcFileList, 'src/' .. proName .. '/**.hpp');
--        table.insert(proSrcFileList, 'src/' .. proName .. '/**.h');
    elseif proLanguage == 'C' then
        templateDir = Path.join(gbsHomeDir, 'template', 'c');
        mainSrc = Path.join(templateDir, proType .. '.c');
        mainDest = Path.join(proSrcDir, 'main.c');
        table.insert(proSrcFileList, Lib.quote('../../src/' .. proName .. '/**.c'));
--        table.insert(proSrcFileList, 'src/' .. proName .. '/**.h');
    end
    
    Lib.assert(mainSrc);
    Lib.assert(mainDest);

    if Path.exists(mainSrc) then
        Lib.assert(Path.copy(mainSrc, mainDest));
    end    

    local proDependson = nil;
    if gbs:hasOpt("depend") then
        local t = Lib.quote(gbs:optarg("depend"));
        proDependson = Lib.join(', ', t);
    end
     
    local slnName        = gbs:solutionName();
    local proKind        = pro:premakeKind();
    local proLang        = pro:premakeLang();
    local proObjDir      = Lib.join('/', '../../../.build', slnName, proName);
    local proTargetDir   = Lib.join('/', '../../../.build');
    local proSrcFiles    = Lib.join(', ', proSrcFileList);
    local proIncludeDirs = Lib.join(', ', proIncludeDirList);
    
    Lib.assert(File.appendLines(proFile, {
              'kind          ' .. Lib.quote(proKind)
            , 'language      ' .. Lib.quote(proLang)
--            , '    location      ' .. Lib.quote(proLocation)
            , 'targetname    ' .. Lib.quote(proName)
            , 'includedirs { ' .. proIncludeDirs .. ' }'
            , 'files { ' .. proSrcFiles .. ' }'
            , 'configuration "debug"'
            , '    objdir    ' .. Lib.quote(proObjDir .. '/debug')
            , '    targetdir ' .. Lib.quote(proTargetDir)
            , '    targetsuffix "-d"'
            , 'configuration "release"'
            , '    objdir    ' .. Lib.quote(proObjDir .. '/release')
            , '    targetdir ' .. Lib.quote(proTargetDir)
        }));    
    
    if proDependson then
        Lib.assert(File.appendLines(proFile, 'dependson { ' .. proDependson ..' }')); 
    end
    
    return true;
end

return CppPlugin;
