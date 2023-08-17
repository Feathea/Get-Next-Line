/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isbesli <isbesli@student.42kocaeli.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/17 17:48:04 by isbesli           #+#    #+#             */
/*   Updated: 2023/08/17 17:48:05 by isbesli          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len] != '\0')
		len++;
	return (len);
}

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*dup;
	int		i;

	i = 0;
	len = ft_strlen(s) + 1;
	dup = (char *)malloc(len);
	if (!dup)
		return (NULL);
	while (*s)
		dup[i++] = *s++;
	dup[i] = '\0';
	return (dup);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len1;
	size_t	len2;
	size_t	i;
	char	*new_str;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	new_str = (char *)malloc(len1 + len2 + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	while (*s1)
		new_str[i++] = *s1++;
	while (*s2)
		new_str[i++] = *s2++;
	new_str[i] = '\0';
	return (new_str);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*res;
	size_t	i;
	size_t	j;
	size_t	calc_len;

	calc_len = ft_strlen(s);
	if (!s || len <= 0)
		return (NULL);
	if (len > calc_len)
		len = calc_len;
	res = malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	i = start;
	j = 0;
	while (i < calc_len && j < len)
	{
		res[j] = s[i];
		i++;
		j++;
	}
	res[j] = '\0';
	return (res);
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
