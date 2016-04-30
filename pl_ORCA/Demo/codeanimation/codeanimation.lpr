program codeanimation;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms,
  codeanifrm in 'codeanifrm.pas' {frmAniCode};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmAniCode, frmAniCode);
  Application.Run;
end.
