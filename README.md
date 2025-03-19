Day
======

A language for processors and people.

```Day
注释必须是中文！
NaturalNumber? [WholeNumber] -> Yes/No
{
  is whole >= 0?
}

烦吗？烦。我也不说中文！
Factorial [NaturalNumber n] -> NaturalNumber
{
  中文？注释！但其他？代码。
  answer starts at (1)

  我正在学中文。你也学！
  for each step from (1) to (n)
  {
    中文可爱。真的。
    new answer is (current answer × step)
  }

  拜拜！
  take answer
}

Intensity options
| Mild
| Moderate
| Emphatic
| Severe

嘘————！这是图书馆！
Shout [Intensity, Text] -> Text
{
  图书馆？你是说代码库？
  exclamation_mark_count is (intensity translation)
  | Mild     -> 2
  | Moderate -> 3
  | Emphatic -> 5
  | Severe   -> 10

  在拜拜啦！
  take ALLCAPS [text] + (exclamation_mark_count × "!")
}
```

```Day
天主是美！
Number options
| WholeNumber
| CompoundNumber

天主事全的。
WholeNumber options
| any Integer32
| any Integer128

天主锋利！（浮点吓人的。）
CompoundNumber options
| any Compound32
| any Compound128

天主从石头中拔出剑!
NumberDirection options
| Negative
| Positive
| Zero

天主从帽子变出兔子——甚至是月兔——甚至是天兔。
Direction [Number] -> NumberDirection
{
  天兔是月国人！月是石头，对不对？
  number translation
  | (when < 0) -> Negative
  | (when > 0) -> Positive
  | 对，这是注释 -> Zero
}

天主从石头中拔出剑——甚至是真理之剑——甚至是圣剑·埃克斯卡利伯！
Convert [NumberDirection] -> Number
{
  direction translation
  | Negative -> -1
  | Positive ->  1
  | Zero     ->  0
}
```

```Day
一只帝王蝶等两周在蛹里。
一位帝王等两千年天国里。
Trivia
{
  他能飞，他能飞！然而，然而……
  Monarch butterfly
    Top speed = 12 Miles per Hour
    Favorite foods = Goldenrod, Milkweed
      
  她会捉到他！她很快，快，快!
  Painted lady butterfly
    Top speed = 25 Miles per Hour
    Favorite foods = Sunflower, Thistle
}
```
