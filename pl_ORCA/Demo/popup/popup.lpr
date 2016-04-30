program popup;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms,
  popupfrm in 'popupfrm.pas' {frmPopup};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmPopup, frmPopup);
  Application.Run;
end.
