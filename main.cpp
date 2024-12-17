#include <iostream>

using namespace std;

struct suffix
{
    int index;   // to store original index of suffix
    int rank[2]; // to store current rank and next rank of suffix
};

bool cmp(struct suffix a, struct suffix b)
{
    return (a.rank[0] == b.rank[0]) ? (a.rank[1] < b.rank[1]) : (a.rank[0] < b.rank[0]);
}

void merge(struct suffix arr[], int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    suffix *L = new suffix[n1];
    suffix *R = new suffix[n2];

    // copy data to temporary arrays
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (cmp(L[i], R[j]))
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }

    delete[] L;
    delete[] R;
}

void mergeSort(struct suffix arr[], int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

int *buildSuffixArray(char *txt, int n)
{
    struct suffix suffixes[n];

    for (int i = 0; i < n; i++)
    {
        suffixes[i].index = i;
        suffixes[i].rank[0] = txt[i];
        suffixes[i].rank[1] = ((i + 1) < n) ? txt[i + 1] : -1;
    }

    mergeSort(suffixes, 0, n - 1);

    int ind[n];
    for (int k = 4; k < 2 * n; k = k * 2)
    {
        int rank = 0;
        int prev_rank = suffixes[0].rank[0];
        suffixes[0].rank[0] = rank;
        ind[suffixes[0].index] = 0;

        for (int i = 1; i < n; i++)
        {
            if (suffixes[i].rank[0] == prev_rank && suffixes[i].rank[1] == suffixes[i - 1].rank[1])
            {
                suffixes[i].rank[0] = rank;
            }
            else
            {
                prev_rank = suffixes[i].rank[0];
                suffixes[i].rank[0] = ++rank;
            }
            ind[suffixes[i].index] = i;
        }

        for (int i = 0; i < n; i++)
        {
            int nextindex = suffixes[i].index + k / 2;
            suffixes[i].rank[1] = (nextindex < n) ? suffixes[ind[nextindex]].rank[0] : -1;
        }

        mergeSort(suffixes, 0, n - 1);
    }

    int *suffixArr = new int[n];
    for (int i = 0; i < n; i++)
    {
        suffixArr[i] = suffixes[i].index;
    }
    return suffixArr;
}

void printArr(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << " ";
    }
    cout << endl;
}

class SuffixArray
{
private:
    char *text;
    int *suffixArr;
    int n;

public:
    SuffixArray(const char *input)
    {
        n = 0;
        while (input[n] != '\0')
            n++;
        text = new char[n + 1];
        for (int i = 0; i < n; ++i)
        {
            text[i] = input[i];
        }
        text[n] = '\0';
    }

    ~SuffixArray()
    {
        delete[] text;
        delete[] suffixArr;
    }

    void ConstructUsingPrefixDoubling()
    {
        suffixArr = buildSuffixArray(text, n);
    }

    void Print() const
    {
        for (int i = 0; i < n; ++i)
        {
            cout << suffixArr[i];
            if (i != n - 1)
                cout << " ";
        }
        cout << endl;
    }
};

int main()
{
    SuffixArray t("ACGACTACGATAAC$");
    t.ConstructUsingPrefixDoubling();
    t.Print(); // Prints: 14 11 12 0 6 3 9 13 1 7 4 2 8 10 5
    return 0;
}
