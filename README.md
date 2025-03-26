Day【天】
======

A language for processors and people.

* “Implicit variables” allow us to refer to the value of any expression by name—without manually binding anything to a variable.

  ```Day
  Normalize [Vector2D] -> NormalizedVector2D
  {
    看「隐式变量」吗？
    translate Length [vector]
    | 1, 0 -> vector
    |      -> vector ÷ length
  }
  ```

  Day recognizes any unambiguous name, such as the two  above:

  * `vector` for the `Vector2D` argument
  * `length` for the expression `Length [vector]`

* “Shelves” can store collections of similar items, just like in real life. Shelves are shared by all code, and their items represent the state of each program.

  _Imagine a big, loving family all sharing a big, loving pantry._

* A built-in “day/night cycle” keeps our programs well-organized and adorably predictable.

  State changes are batched together during the day, and they’re applied together during the night, once the whole program has had a chance to examine the previous batch of changes.

  _Each day, the big, loving family carefully prepares a feast. They all wait until sundown to start eating together!_

* Chinese is automatically interpreted as commentary, not as code itself. We can write commentary anywhere in our code without affecting the behavior of our programs.

Welcome to Day. 早上好！欢迎来到天。

```Day
注释必须是中文！
NaturalNumber? [WholeNumber] -> Yes/No
{
  whole >= 0?
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
    new answer is (old answer × this step)
  }

  拜拜！
  give answer
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
  give ALLCAPS [text] + (exclamation_mark_count × "!")
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
  Numbers x, y
]

欢迎函数！
Normalize [Vector2D] -> NormalizedVector2D
{
  看「隐式变量」吗？
  translate Length [vector]
  | 1, 0 -> vector
  |      -> vector ÷ length
}

NormalizedVector2D? [Vector2D] -> Yes/No
{
  Length [vector] == 1?
}

隐式变量：「first，second」
DotProduct [Vector2D, Vector2D] -> Vector2D
[
  我们在方括号里，不在大括号! 
  x: first x × second x
  y: first y × second y
]

因为「×」很可爱。
operator (×) [Vector2D, Vector2D] -> Vector2D
{
  DotProduct [first, second]
}

operator (×) [Vector2D, Number scalar] -> Vector2D
[
  x: vector x × scalar
  y: vector y × scalar
]

见上文！
operator (÷) [Vector2D, NonZero divisor] -> Vector2D
[
  x: vector x ÷ divisor
  y: vector y ÷ divisor
]

operator (+) [Vector2D, Vector2D] -> Vector2D
[
  x: first x + second x
  y: first y + second y
]

operator (-) [Vector2D, Vector2D] -> Vector2D
[
  x: first x - second x
  y: first y - second y
]

RealNumber? [Number] -> Yes/No
{
  number >= 0?
}

Length [Vector2D] -> RealNumber
{
  拜拜！谨慎嵌套！
  SquareRoot
  [
    Square [vector x] + Square [vector y]
  ]
}
```

```Day
早啊，植物！醒醒吧！
plan gathering
{
  这些是架子。
  gather Gardeners as harvesters
  gather Plants as crops
  
  crop criteria
    awake: No
    wake_cycle: Diurnal
    heart >= Good
  
  relationship criteria
    在附近！
    Distance [crop location, harvester location] <= 40 Miles
  
  update related crops
  [
    伸展啦，小苗苗！
    is_awake: Yes
    
    我们重置这些。
    harvesters: related harvesters
    xp: Sum [related harvesters, xp] 「XP」是什么？经验值！
  
    我们不重置这些！
    hp: old hp + 1,000                    「HP」？生命值！
    mp: old mp + 10,000                   「MP」？魔法值！
    pp: old pp + 10,000,000               「PP」？祈祷值！
    fp: old fp + 10,000,000,000,000,000   「FP」？信心值！
  ]
  
  update related harvesters
  [
    xp: old xp + (120 × Count [related crops])
  ]
}
```

```Day
没有中间吗？
Goodness options
| CartoonishlyBad
| Bad
| Good
| CartoonishlyGood

这个函数允许比较。
Convert [Goodness] -> Number
{
  goodness translation
  | CartoonishlyBad  -> -2
  | Bad              -> -1
  | Good             ->  1
  | CartoonishlyGood ->  2  
}

Distance [Vector2D, Vector2D] -> RealNumber
{
  Length [first - second] 
}
```

```Day
stock Plants
[   
  Yes/No          awake?
  Yes/No          blooms?
  WakeCycle       wake_cycle
  Soil            favorite_soil
  PronounFamily   favorite_pronouns
  Goodness        heart
  Patience        head
  NaturalNumbers  hp, mp, pp, fp, xp
]

Plant relationships
[
  Gardeners as harvesters
]

PronounFamily options
| He/Him
| She/Her
| They/Them
  
WakeCycle options
| Diurnal
| Nocturnal
| Crepuscular

魔法降临。光明入侵黑暗。天国侵略世界！
Soil options
| PurpleSand
| GoldClay
| Loam
```
