program d2designer;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms,
  designform in 'designform.pas' {frmDesigner};

begin
  Application.Initialize;
  Application.CreateForm(TfrmDesigner, frmDesigner);
  Application.Run;
end.
