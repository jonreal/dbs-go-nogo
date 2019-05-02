numOfReaches = 36; % must be multiple of 4 & 3


% create all possible task
colors = zeros(numOfReaches,1);
gonogo = zeros(numOfReaches,1);
sequence = (1:numOfReaches)';

for i=1:(numOfReaches/3)
  colors(i) = 1;
  colors(i+(numOfReaches/3)) = 2;
  colors(i+2*(numOfReaches)/3) = 3;
end

for i=1:4:numOfReaches
  gonogo(i) = 0;
  gonogo(i+1) = 0;
  gonogo(i+2) = 0;
  gonogo(i+3) = 1;
end

[sequence, colors, gonogo]

sequence_ = zeros(numOfReaches,1);
colors_ = zeros(numOfReaches,1);
gonogo_ = zeros(numOfReaches,1);

maxRandi = numOfReaches;
for i=1:numOfReaches
  randIndx = randi([1,maxRandi]);
  sequence_(i) = sequence(randIndx);
  colors_(i) = colors(randIndx);
  gonogo_(i) = gonogo(randIndx);
  for j=(randIndx+1):maxRandi
    colors(j-1) = colors(j);
    gonogo(j-1) = gonogo(j);
    sequence(j-1) = sequence(j);
  end
  maxRandi = maxRandi - 1;
end


[sequence_, colors_, gonogo_]


