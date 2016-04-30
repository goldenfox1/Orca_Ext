{ This file was automatically created by Typhon. Do not edit!
  This source is only used to compile and install the package.
 }

unit lz_images;

interface

uses
  LazPNG, LazPNM, LazJPG, LazBMP, LazTGA, LazXPM, LazarusPackageIntf;

implementation

procedure Register;
begin
  RegisterUnit('LazTGA', @LazTGA.Register);
end;

initialization
  RegisterPackage('lz_images', @Register);
end.
