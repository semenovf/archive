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
    local proName = pro:name();
    local solutionFile = Path.join(".gbs", "solution.gbs");

    local solutionName = pro:solutionName();
    local proKind      = pro:premakeKind();
    local proLang      = pro:premakeLang();
    local proLocation  = "../../.build/" .. solutionName .. '/' .. proName;
        
    Lib.assert(File.appendLines(solutionFile, {
              '    kind     '   .. Lib.quote(proKind)
            , '    language '   .. Lib.quote(proLang)
            , '    location '   .. Lib.quote(proLocation)
            , '    targetname ' .. Lib.quote(proName)
            , '    configuration "debug"'
            , '        targetdir ' .. Lib.quote(proLocation .. '/debug')
            , '        targetsuffix "-d"'
            , '    configuration "release"'
            , '        targetdir ' .. Lib.quote(proLocation .. '/release')
        }));    
    
    local proLanguage = pro:language();
    local proType = pro:type();
    
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
    end
    
    local templateDir = nil;
    local mainSrc = nil;
    local mainDest = nil;
    
    if proLanguage == 'C++' then
        templateDir = Path.join(pro:homeDir(), 'template', 'cpp');
        mainSrc = Path.join(templateDir, proType .. '.cpp');
        mainDest = Path.join(proSrcDir, 'main.cpp');
    elseif proLanguage == 'C' then
        templateDir = Path.join(pro:homeDir(), 'template', 'c');
        mainSrc = Path.join(templateDir, proType .. '.c');
        mainDest = Path.join(proSrcDir, 'main.c');
    end
    
    Lib.assert(mainSrc);
    Lib.assert(mainDest);

    if Path.exists(mainSrc) then
        Lib.assert(Path.copy(mainSrc, mainDest));
    end    
    
    return true;
end

return CppPlugin;
