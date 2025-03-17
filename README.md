Day
======

A language for processors and people.

```Day
注视必须是中文！
NaturalNumber? [Integer] -> Yes/No
{
  is integer >= 0?
}

烦吗？烦。我也不说中文！
Factorial [NaturalNumber n] -> NaturalNumber
{
  中文？注视！但其他？代码。
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
