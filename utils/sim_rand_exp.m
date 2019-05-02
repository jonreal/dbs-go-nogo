% Parameters
T = 120000;
minSpace = 3000;
numOfReaches = 12;

% Generate transitions times
maxRandNum = T/minSpace;
transition = zeros(2*numOfReaches,1);
notunique = 0;
for i=1:(2*numOfReaches)
  while (1)
    randnum = randi([1,maxRandNum]);
    for j=1:(i-1)
      if (randnum == transition(j))
        notunique = 1;
        break;
      end
      notunique = 0;
    end
    if (notunique)
      continue;
    else
      transition(i) = randnum;
      break;
    end
  end
end
transition = sort(transition)*minSpace;




% Generate color and go no oghoice
choice_bag = [ [ones(numOfReaches/6,1), ones(numOfReaches/6,1).*0]; ...
               [ones(numOfReaches/6,1), ones(numOfReaches/6,1).*1]; ...
               [ones(numOfReaches/6,1).*2, ones(numOfReaches/6,1).*0]; ...
               [ones(numOfReaches/6,1).*2, ones(numOfReaches/6,1).*1]; ...
               [ones(numOfReaches/6,1).*3, ones(numOfReaches/6,1).*0]; ...
               [ones(numOfReaches/6,1).*3, ones(numOfReaches/6,1).*1;] ];

% option 1
used = zeros(numOfReaches,1);
colors = zeros(numOfReaches,1);
gonogo = zeros(numOfReaches,1);

for i=1:numOfReaches;
  while(1)
    randInx = randi([1,numOfReaches]);
    if (used(randInx) == 1)
      continue;
    else
      used(randInx) = 1;
      colors(i) = choice_bag(randInx,1);
      gonogo(i) = choice_bag(randInx,2);
      break;
    end
  end
end
gonogo = gonogo*0.5;
task_type = [colors, gonogo];


% Sim
trans_iter = 1;
task_iter = 1;
taskOn = 0;
color_cmd = zeros(1,T);
gonogo_cmd = zeros(1,T);
for t=1:T

  % Transitions
  if ((t >= transition(trans_iter)) && (trans_iter < 2*numOfReaches))
    trans_iter = trans_iter + 1;

    % task flag
    if (taskOn == 0)
      taskOn = 1;
    elseif (taskOn == 1)
      taskOn = 0;
      task_iter = task_iter + 1;
    end
  elseif (t >= transition(end))
    taskOn = 0;
  end

  % Commands
  if (taskOn)
    color_cmd(t) = task_type(task_iter,1);
    gonogo_cmd(t) = task_type(task_iter,2);
  else
    color_cmd(t) = 0;
    gonogo_cmd(t) = 0;
  end

end

figure; hold all;
  plot((1:T)./1000,color_cmd);
  plot((1:T)./1000,gonogo_cmd);
  ylim([0,5]);



