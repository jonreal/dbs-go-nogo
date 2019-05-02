


maxTaskTime = 6000;
minTransition = 3000;


t1 = randi([1,maxTaskTime]);
while(1)
  t2 = randi([1,maxTaskTime]);
  if abs(t1 - t2) < 3000
    continue;
  else
    break;
  end
end

if (t1 > t2)
  temp = t1;
  t1 = t2;
  t2 = temp;
end



