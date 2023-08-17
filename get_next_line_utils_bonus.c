/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isbesli <isbesli@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 17:48:06 by isbesli           #+#    #+#             */
/*   Updated: 2023/08/17 17:48:07 by isbesli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}

char *ft_strdup(const char *s)
{
    size_t len; // Verilen karakter dizisinin uzunluğu (NULL karakteri dahil)
    char *dup;   // Kopyalanan karakter dizisinin saklanacağı bellek alanı işaretçisi
    int i;       // Döngü sayacı

    i = 0; // Döngü sayacını başlangıç değeri olarak ayarla
    len = ft_strlen(s) + 1; // Verilen karakter dizisinin uzunluğunu hesapla (NULL karakteri dahil)

    // Kopyalanan karakter dizisi için bellekte gerekli alan ayrılır.
    dup = (char *)malloc(len);

    // Bellek ayrılma hatası durumunda NULL döner.
    if (!dup)
        return (NULL);

    // Verinin karakterleri bellek alanına kopyalanır.
    while (*s)
        dup[i++] = *s++;

    dup[i] = '\0'; // Kopyalanan karakter dizisinin sonuna NULL karakteri eklenir.

    return (dup); // Kopyalanan karakter dizisi işaretçisi döndürülür.
}


char *ft_strjoin(char const *s1, char const *s2)
{
    size_t len1; // İlk karakter dizisinin uzunluğu
    size_t len2; // İkinci karakter dizisinin uzunluğu
    size_t i;    // Döngü sayacı
    char *new_str; // Yeni birleştirilmiş karakter dizisi için işaretçi

    // Eğer s1 veya s2 karakter dizileri boş (NULL) ise, fonksiyon NULL döner.
    if (!s1 || !s2)
        return (NULL);

    // İlk karakter dizisinin uzunluğu hesaplanır.
    len1 = ft_strlen(s1);

    // İkinci karakter dizisinin uzunluğu hesaplanır.
    len2 = ft_strlen(s2);

    // Yeni birleştirilmiş karakter dizisi için bellekte gerekli alan ayrılır.
    new_str = (char *)malloc(len1 + len2 + 1);

    // Bellek ayrılma hatası durumunda NULL döner.
    if (!new_str)
        return (NULL);

    i = 0; // Döngü sayacı başlangıç değeri
    while (*s1)
        new_str[i++] = *s1++; // İlk karakter dizisinin karakterleri yeni diziye kopyalanır.

    while (*s2)
        new_str[i++] = *s2++; // İkinci karakter dizisinin karakterleri yeni diziye kopyalanır.

    new_str[i] = '\0'; // Yeni dizenin sonuna NULL karakteri eklenir.

    return (new_str); // Yeni birleştirilmiş karakter dizisi döndürülür.
}

char *ft_substr(char const *s, unsigned int start, size_t len)
{
    char *res; // Oluşturulan alt dize (substring) için işaretçi
    size_t i; // Döngü sayaçları
    size_t j;
    size_t calc_len; // Verilen karakter dizisinin uzunluğu

    // Verilen karakter dizisi boş ise veya istenen uzunluk 0 ise, NULL döndürülür.
    calc_len = ft_strlen(s);
    if (!s || len <= 0)
        return (NULL);

    // İstenen uzunluk verilen dizinin uzunluğundan büyükse, istenen uzunluğu dizinin uzunluğu ile değiştir.
    if (len > calc_len)
        len = calc_len;

    // Oluşturulacak alt dize için bellekte yer ayrılır.
    res = malloc(sizeof(char) * (len + 1));

    // Bellek ayrılma hatası durumunda NULL döndürülür.
    if (!res)
        return (NULL);

    i = start; // Döngü için start değeri atanır.
    j = 0; // Alt dize indeksi
    while (i < calc_len && j < len)
    {
        // Verinin başlangıç konumundan itibaren alt dizeye karakterler kopyalanır.
        res[j] = s[i];
        i++;
        j++;
    }
    res[j] = '\0'; // Alt dizenin sonuna NULL karakteri eklenir.

    return (res); // Oluşturulan alt dize (substring) döndürülür.
}


void *ft_free_stash(char **stash, int create_line)
{
    char *line;

    // 1. Eğer stash işaretçisi zaten NULL ise (yani bellek alanı zaten temizlendiyse),
    // fonksiyon doğrudan NULL döner ve işlem sonlandırılır.
    if (!*stash)
        return (NULL);

    // 2. Eğer create_line değeri 0 ise (yani yeni bir satır oluşturulmayacaksa),
    if (create_line == 0)
    {
        // 2.1. Bellek alanını serbest bırakır (free) ve stash işaretçisini NULL yaparak bellek alanını temizler.
        free(*stash);
        *stash = NULL;

        // 2.2. Fonksiyon NULL döner, çünkü yeni bir satır oluşturulmayacak ve bellek temizlenmiştir.
        return (NULL);
    }
    // 3. Eğer create_line değeri 1 ise (yani yeni bir satır oluşturulacaksa),
    else if (create_line == 1)
    {
        // 3.1. Stash'teki veriyi kopyalayarak yeni bir bellek alanı oluşturur.
        line = ft_strdup(*stash);

        // 3.2. Bellek alanını serbest bırakır (free) ve stash işaretçisini NULL yaparak bellek alanını temizler.
        free(*stash);
        *stash = NULL;

        // 3.3. Oluşturulan yeni bellek alanının adresini döndürerek bu yeni satırı döner.
        return (line);
    }

    // 4. Eğer create_line değeri hem 0 hem de 1 değilse (bu durumda 0 ve 1 dışında bir değer gelebilir),
    // fonksiyon NULL döner, çünkü geçerli bir durumu işlemek için gerekli bilgi yoktur.
    return (NULL);
}