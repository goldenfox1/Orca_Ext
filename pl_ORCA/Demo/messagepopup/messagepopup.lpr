program messagepopup;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms,
  messagepopupfrm in 'messagepopupfrm.pas' {frmMessagePopup};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmMessagePopup, frmMessagePopup);
  Application.Run;
end.
