program ExampleInput;

{$ifdef MSWINDOWS}{$apptype CONSOLE}{$endif}
{$ifdef FPC}{$mode OBJFPC}{$H+}{$endif}

uses
  SysUtils, IPConnection, BrickletIO16V2;

type
  TExample = class
  private
    ipcon: TIPConnection;
    io: TBrickletIO16V2;
  public
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change XYZ to the UID of your IO-16 Bricklet 2.0 }

var
  e: TExample;

procedure TExample.Execute;
var value: TArray0To15OfBoolean;
begin
  { Create IP connection }
  ipcon := TIPConnection.Create;

  { Create device object }
  io := TBrickletIO16V2.Create(UID, ipcon);

  { Connect to brickd }
  ipcon.Connect(HOST, PORT);
  { Don't use device before ipcon is connected }

  { Get current value }
  value := io.GetValue;

  WriteLn(Format('Channel 0 [A0]: %d', [value[0]]));
  WriteLn(Format('Channel 1 [A1]: %d', [value[1]]));
  WriteLn(Format('Channel 2 [A2]: %d', [value[2]]));
  WriteLn(Format('Channel 3 [A3]: %d', [value[3]]));
  WriteLn(Format('Channel 4 [A4]: %d', [value[4]]));
  WriteLn(Format('Channel 5 [A5]: %d', [value[5]]));
  WriteLn(Format('Channel 6 [A6]: %d', [value[6]]));
  WriteLn(Format('Channel 7 [A7]: %d', [value[7]]));
  WriteLn(Format('Channel 8 [B0]: %d', [value[8]]));
  WriteLn(Format('Channel 9 [B1]: %d', [value[9]]));
  WriteLn(Format('Channel 10 [B2]: %d', [value[10]]));
  WriteLn(Format('Channel 11 [B3]: %d', [value[11]]));
  WriteLn(Format('Channel 12 [B4]: %d', [value[12]]));
  WriteLn(Format('Channel 13 [B5]: %d', [value[13]]));
  WriteLn(Format('Channel 14 [B6]: %d', [value[14]]));
  WriteLn(Format('Channel 15 [B7]: %d', [value[15]]));

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
