% This file is part of jack_playrec
% Copyright (C) 2021 Hörtech gGmbH
%
%  This program is free software: you can redistribute it and/or modify
%  it under the terms of the GNU General Public License as published by
%  the Free Software Foundation, either version 3 of the License, or
%  (at your option) any later version.
%
%  This program is distributed in the hope that it will be useful,
%  but WITHOUT ANY WARRANTY; without even the implied warranty of
%  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.

function set_jack_playrec_path()
% set_jack_playrec_path()
% Checks if jack_playrec can be found in the PATH
% And amends the PATH with the operating system's
% default jack_playrec installation directory
% if it is not found. If jack_playrec can not be
% found after amendment, exits with an error message
  if(check_playrec()==0)
    return
  end

  if ismac % Macos
    bin='/usr/local/bin';
    delim=':';
  elseif ispc % Windows
    bin='C:\Program Files\jack_playrec\bin';
    delim=';';
  else % Linux
    bin='/usr/bin';
    delim=':';
  end
  curPATH=getenv('PATH');
  if(isempty(strfind(curPATH,bin)))
    setenv('PATH',[getenv('PATH') delim bin])
  end
  if(check_playrec()~=0)
    error('jack_playrec was not found in PATH and/or default installation directory!'...
            ' Please add the location of jack_playrec to the PATH!');
end

function res = check_playrec()
% res=check_playrec()
% Checks if jack_playrec can be found in the PATH
% res: 0 if jack_playrec could be found, non-zero otherwise
  if ispc
    [res,a]=system('where jack_playrec');
  else
    [res,a]=system('which jack_playrec');
  end
end
