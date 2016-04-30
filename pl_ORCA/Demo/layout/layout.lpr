program layout;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms,
  layoutdemoform in 'layoutdemoform.pas' {frmLayout};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmLayout, frmLayout);
  Application.Run;
end.
