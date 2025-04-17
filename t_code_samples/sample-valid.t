enum Gospel
{
  马太，不是马特！
  Matthew
  Mark
  Luke
  John
}

Line SignatureLine(Gospel)
{
  return switch (gospel)
  {
    Matthew: 'Blessed are the merciful, for they will receive mercy.'
    Mark: 'And he sternly ordered them not to tell anyone about him.'
    Luke: 'Then the demons came out of the man and entered the pigs.'
    John: 'Verily, verily, I say unto you! Before Abraham was, I am.'
  }
}