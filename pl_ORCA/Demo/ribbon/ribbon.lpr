program ribbon;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms,
  ribbonfrm in 'ribbonfrm.pas' {frmRibbon};

begin
  Application.Initialize;
  Application.CreateForm(TfrmRibbon, frmRibbon);
  Application.Run;
end.
