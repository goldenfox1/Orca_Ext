program vectorimage;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, 
  vectorimagefrm in 'vectorimagefrm.pas' {frmVectorImage};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmVectorImage, frmVectorImage);
  Application.Run;
end.
