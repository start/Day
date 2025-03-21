Day 天
======

A language for processors and people.

* “Auto-variables” allow us to **unambiguously** refer to the value of any expression using a sensible name—without having to explicitly create a variable and bind it to the expression.
* “Shelves” can store collections of similar items, just like in real life. Shelves are shared by all code, and their items represent the state of each program. _(Imagine a big, loving family all sharing a big, loving pantry.)_
* A built-in “day/night cycle” keeps our programs well-organized and adorably predictable. State changes are batched together and applied together once the whole program has had a chance to examine the previous batch of changes. _(Each day, the big, loving family carefully prepares a feast. They all wait until sundown to start eating together!)_
* Chinese is automatically interpreted as commentary, not as code itself. We can write commentary anywhere in our code without affecting the behavior of our programs.

Welcome to Day. 早上好！欢迎来到天。

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

```Day

无方法。不允许方法。
Vector2D attributes
[
  Number x
  Number y
]

欢迎函数！
Normalize [Vector2D] -> NormalizedVector2D
{
  看“自变量”吗？
  translate Length [vector]
  | 1, 0 -> vector
  |      -> vector ÷ length
}

NormalizedVector2D? [Vector2D] -> Yes/No
{
  is Length [vector] == 1?
}

方括号，无大括号。
DotProduct [Vector2D, Vector2D] -> Vector2D
[
  陶艺中，刮痕连接！
  x is (first::x × second::x)
  y is (first::y × second::y)
]

因为“×”很可爱。
operator (×) [Vector2D, Vector2D] -> Vector2D
{
  DotProduct [first, second]
}

operator (×) [Vector2D, Number scalar] -> Vector2D
[
  x is (vector::x × scalar)
  y is (vector::y × scalar)
]

见上文！
operator (÷) [Vector2D, NonZero divisor] -> Vector2D
[
  x is (vector::x ÷ divisor)
  y is (vector::y ÷ divisor)
]

operator (+) [Vector2D, Vector2D] -> Vector2D
[
  x is (first::x + second::x)
  y is (first::y + second::y)
]

operator (-) [Vector2D, Vector2D] -> Vector2D
[
  x is (first::x - second::x)
  y is (first::y - second::y)
]

RealNumber? [Number] -> Yes/No
{
  is number >= 0?
}

Length [Vector2D] -> RealNumber
{
  拜拜！谨慎嵌套！
  SquareRoot
  [
    Square [vector::x] + Square [vector::y]
  ]
}
```
