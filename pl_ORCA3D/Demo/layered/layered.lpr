program layered;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, pl_orca,
  layerfrm in 'layerfrm.pas' {frmLayered};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmLayered, frmLayered);
  Application.Run;
end.
