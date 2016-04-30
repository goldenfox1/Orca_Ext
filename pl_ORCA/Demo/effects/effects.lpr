program effects;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms,
  effectsfrm in 'effectsfrm.pas' {frmEffects};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmEffects, frmEffects);
  Application.Run;
end.
