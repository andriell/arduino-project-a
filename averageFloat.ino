class averageFloat
{
  private:
    byte i = 0;
    byte size = 10;
    boolean full = false;
    float data[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  public:
    void put(float f) {
      data[i] = f;
      i++;
      if (i >= size) {
        i = 0;
        full = true;
      }
    }

    float read()
    {
      float r = 0;
      if (full) {
        for (byte j = 0; j < size; j++) {
          r += data[j];
        }
        r = r / size;
      } else {
        for (byte j = 0; j < i; j++) {
          r += data[j];
        }
        r = r / i;
      }
      return r;
    }
};
