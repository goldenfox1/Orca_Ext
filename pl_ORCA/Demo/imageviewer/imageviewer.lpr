program imageviewer;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms,
  imageviewerfrm in 'imageviewerfrm.pas' {frmImageViewer};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmImageViewer, frmImageViewer);
  Application.Run;
end.                                                                                                   
