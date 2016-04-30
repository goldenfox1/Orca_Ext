{%mainunit ../orca_scene3d.pas}
 
{**********************************************************************
 Package pl_ORCA3D.pkg
 This unit is part of CodeTyphon Studio  (http://www.pilotlogic.com/)
***********************************************************************}

//============= TD3Filter ====================================

class function TD3Filter.GetFileTypes:string;
begin
  Result:='';
end;

class function TD3Filter.GetImageSize(const AFileName:string):TD3Point;
begin
  Result:=d3Point(0,0,0);
end;       
         
           
