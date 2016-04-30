{ This file was automatically created by Typhon IDE. Do not edit!
  This source is only used to compile and install the package.
 }

unit pl_ORCA;

interface

uses
  AllOrcaRegister, orca_scene2d, LazarusPackageIntf;

implementation

procedure Register;
begin
  RegisterUnit('AllOrcaRegister', @AllOrcaRegister.Register);
end;

initialization
  RegisterPackage('pl_ORCA', @Register);
end.
