% VisualMatch function display the point matching results
% Parameters:
% fn        -- Fiel name of the point matching result
% bDisplay  -- Display the match links or not. Default is to display matching
% links
% nShift    -- Shift a shape in display. Default value is 0
function VisualLink( fn, bDisplay, ShiftX, ShiftY )
if nargin == 1
    ShiftX = 0;
    ShiftY = 0;
    bDisplay = 1;
elseif nargin == 2
    ShiftX = 0;
    ShiftY = 0;
elseif nargin == 3
    ShiftY = ShiftX;
end

fid = fopen( fn, 'rt' );
%Read model points
tline = fgets( fid );
pos = strfind( tline, '=');
tline = tline( 1, pos+1:size(tline,2));
nPt = sscanf( tline, '%d' );
[Data, count]= fscanf( fid, '%f', [3 nPt] );
tline = fgets( fid );
X1 = Data(2, :)';
Y1 = Data(3, :)';

%Read deformed points
tline = fgets( fid );
pos = strfind( tline, '=');
tline = tline( 1, pos+1:size(tline,2));
nPt = sscanf( tline, '%d' );
[Data, count] = fscanf( fid, '%f', [3 nPt] );
tline = fgets( fid );
X2 = Data(2, :)';
Y2 = Data(3, :)';
X2 = X2 + ShiftX;
Y2 = Y2 + ShiftY;

%Read point matching results
tline = fgets( fid );
pos = strfind( tline, '=');
tline = tline( 1, pos+1:size(tline,2));
nMatch = sscanf( tline, '%d' );
[Match, count] = fscanf( fid, '%f', [2 nMatch] );
fclose( fid );
Match = Match'+1;

index = Match(:,1);      %Get all points matching to normal points
in = index( index~=0 );
X3 = X1(in);  
Y3 = Y1(in); 
figure;
plot( X1, Y1, 'go');    %Display all points as un-matched points with green color
hold on;
plot( X3, Y3, 'ro');    %Display matched points with blue color
axis xy;
axis equal;

index = Match(:,2);      %Get all points matching to normal points
in = index( index~=0 );
nMatch = size(in, 1);
X3 = X2(in);            
Y3 = Y2(in);
plot( X2, Y2, 'g+');    %Display all points as un-matched points with green color
plot( X3, Y3, 'b+');    %Display matched points with read color
h = title([fn '  ' num2str(nMatch) '  matched pairs (deform +, model o)']);
set( h, 'Interpreter', 'none' );

%Display matched pairs with lines
if bDisplay == 1
	for i=1:nMatch
        if Match(i, 2) == 0 | Match(i,1) == 0     %Dumping point
            continue;
        end
        plot( [X1(Match(i,1)) X2(Match(i,2))], [Y1(Match(i,1)) Y2(Match(i,2))], 'k-');
	end
end

%Adjust the size of display
minX = min([X1; X2]);
maxX = max([X1; X2]);
w = maxX - minX;
minY = min([Y1; Y2]);
maxY = max([Y1; Y2]);
h = maxY - minY;
axis([minX-0.05*w, maxX+0.05*w, minY-0.05*h, maxY+0.05*h]);

%exportfig(gcf, 'Sig001_1_5.tif', 'format', 'tiff', 'color', 'rgb', 'height', 6, 'resolution', 300 );