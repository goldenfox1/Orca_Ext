{ This file was automatically created by Typhon IDE. Do not edit!
  This source is only used to compile and install the package.
 }

unit pl_orca3d;

interface

uses
  AllOrca3DRegister, orca_scene3d, LazarusPackageIntf;

implementation

procedure Register;
begin
  RegisterUnit('AllOrca3DRegister', @AllOrca3DRegister.Register);
end;

initialization
  RegisterPackage('pl_orca3d', @Register);
end.
