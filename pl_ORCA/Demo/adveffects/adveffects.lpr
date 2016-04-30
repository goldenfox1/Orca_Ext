program adveffects;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, Forms,
  adveffectsfrm;

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TfrmAdvEffects, frmAdvEffects);
  Application.Run;
end.
