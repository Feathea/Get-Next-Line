/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isbesli <isbesli@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 17:48:01 by isbesli           #+#    #+#             */
/*   Updated: 2023/08/17 17:48:16 by isbesli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char *ft_copy_to_stash(char *stash, char *buf)
{
    char *res;  // Geçici bir bellek bloğu

    res = 0;    // Geçici bellek bloğunu başlangıçta NULL olarak atayın
    if (!stash && buf)  // Eğer stash boşsa ve buf verisi varsa
    {
        res = ft_strdup(buf);  // buf verisini kopyalayarak res'e atayın
        if (!res)   // Bellek ayırma hatası durumunda NULL döndür
            return (NULL);
        return (res);   // Kopyalanan veriyi döndür
    }
    res = ft_strjoin(stash, buf);   // stash ve buf'u birleştirerek res'e atayın
    ft_free_stash(&stash, 0);       // Eski stash'i temizleyin (bellek sızıntısı önlemek için)
    return (res);   // Birleştirilmiş veriyi döndür
}

static int ft_have_nl(char *s)
{
    size_t i;

    // İlk olarak, gelen karakter dizisinin (string) boş olup olmadığını kontrol ediyoruz.
    // Eğer boşsa, '\0' karakteri yani null terminator olmadığı için '\n' karakterini içermiyor demektir.
    if (!s)
        return (0);

    // Bir sayaç (i) tanımlanıyor ve -1 ile başlatılıyor.
    // Döngüde başlangıç değeri -1 olarak seçilmiş, böylece döngü başladığında i++ ile 0'ıncı indeksten başlayacaktır.
    i = -1;

    // Karakter dizisinin sonuna kadar dönecek bir döngü başlıyor.
    // Bu döngü, karakter dizisinin her bir karakterini sırayla kontrol eder.
    while (s[++i] != '\0') {
        // Eğer mevcut karakter '\n' (newline) karakterine eşitse,
        // bu, karakter dizisi içerisinde yeni satır karakteri bulunduğu anlamına gelir.
        // Bu durumda fonksiyon 1 değerini döndürerek "evet, yeni satır karakteri bulundu" demektedir.
        if (s[i] == '\n')
            return (1);
    }

    // Eğer döngü sonuna kadar gelindi ve '\n' karakteri bulunmadıysa,
    // fonksiyon 0 değerini döndürerek "hayır, yeni satır karakteri bulunmuyor" demektedir.
    return (0);
}

static char *ft_extract_line(char *stash)
{
    char *line;     // Yeni satırı saklamak için bellek alanı
    size_t i;       // Stash içinde dolaşmak için sayaç
    size_t j;       // Line dizisine veriyi kopyalamak için sayaç

    i = 0;          // Stash içinde gezinmeye başlamak için i'yi sıfırla
    if (!stash)     // Stash boşsa, yani veri yoksa, hemen NULL döndür
        return (0);

    // Stash içindeki veriyi '\n' karakterine kadar tara
    while (stash[i] != '\n')
        i++;

    // '\n' karakteri dahil olarak satırın uzunluğunu (i + 1) alıp bellek ayır
    line = malloc(sizeof(char) * (i + 2));
    if (!line)      // Bellek ayırma hatası durumunda NULL döndür
        return (0);

    j = 0;          // Yeni line dizisine veriyi kopyalamak için j'yi sıfırla
    while (j < i + 1)   // '\n' karakteri dahil olarak veriyi kopyala
    {
        line[j] = stash[j];
        j++;
    }
    line[j] = '\0'; // Sonuna NULL karakteri ekle (C-string bitirme işareti)

    return (line);  // Oluşturulan satırı döndür
}

static char *ft_recreate_stash(char *stash)
{
    size_t i;      // Diziyi tararken kullanılacak sayaç
    char *res;     // Yeni oluşturulacak bellek bloğu

    i = 0;         // Sayaç başlangıç değeri ile sıfırlanır
    if (!stash)    // Eğer stash NULL ise, hemen NULL döndür
        return (NULL);

    while (stash[i] != '\n')  // '\n' karakterine kadar dizi taranır
        i++;

    if (stash[i + 1] == '\0') // Eğer '\n' karakterinden sonraki karakter '\0' ise
        return (ft_free_stash(&stash, 0)); // stash'i temizle ve NULL döndür

    // stash içinden '\n' sonrasını alarak yeni bellek bloğu oluştur
    res = ft_substr(stash, i + 1, ft_strlen(stash));
    if (!res)   // Bellek ayırma hatası durumunda
    {   
        ft_free_stash(&stash, 0); // stash'i temizle
        return (NULL);            // NULL döndür
    }

    ft_free_stash(&stash, 0); // stash'i temizle
    return (res);             // Yeni bellek bloğunu döndür
}

char *get_next_line(int fd)
{
    char buf[BUFFER_SIZE + 1]; // Okunan verinin geçici olarak saklanacağı karakter dizisi (buffer)
    long ret; // Okunan bayt sayısını tutan değişken
    static char *stash[NUM_OF_FD]; // Her dosya tanımlayıcısı için ayrı bir bellek alanı saklayan dizi
    char *line; // Okunan bir satırın son halini tutan işaretçi

    line = 0; // Satır işaretçisini başlangıçta NULL olarak ayarla
    ret = BUFFER_SIZE; // ret değerini BUFFER_SIZE olarak ayarla

    // Hatalı dosya tanımlayıcısı veya geçersiz BUFFER_SIZE kontrolü yapılır.
    if (fd < 0 || BUFFER_SIZE <= 0)
        return (ft_free_stash(&stash[fd], 0));

    // Okuma işlemi ve veri çıkarma döngüsü başlar.
    while (ret > 0)
    {
        // Dosyadan veri okunur ve okunan bayt sayısı ret değişkenine atanır.
        ret = read(fd, buf, BUFFER_SIZE);

        // Okuma hatası veya veri yoksa ve stash bellek alanı da boşsa veya
        // okuma hatası (ret == -1) oluştuysa, bellek temizlenir ve NULL döner.
        if ((ret <= 0 && !stash[fd]) || ret == -1)
            return (ft_free_stash(&stash[fd], 0));

        // Okunan verinin sonuna NULL karakteri eklenerek C-string formatına dönüştürülür.
        buf[ret] = '\0';

        // Okunan veriyi stash bellek alanına kopyalar.
        stash[fd] = ft_copy_to_stash(stash[fd], buf);

        // Stash bellek alanında yeni satır karakteri var mı diye kontrol edilir.
        if (ft_have_nl(stash[fd]))
        {
            // Eğer yeni satır karakteri bulunuyorsa, stash'teki veriden yeni bir satır çıkarılır.
            line = ft_extract_line(stash[fd]);

            // Satır çıkarılma işlemi başarılı değilse, bellek temizlenir ve NULL döner.
            if (!line)
                return (ft_free_stash(&stash[fd], 0));

            // Stash bellek alanı yeniden oluşturulur ve çıkarılan satır döner.
            return (stash[fd] = ft_recreate_stash(stash[fd]), line);
        }
    }

    // Döngü tamamlandığında ve okuma işlemi tamamlandığında,
    // bellek temizlenir ve işlem tamamlandığına dair 1 döner.
    return (ft_free_stash(&stash[fd], 1));
}

