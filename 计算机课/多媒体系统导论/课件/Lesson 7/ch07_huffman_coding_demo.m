close all, clear all
%% 
%%data setting
symbols = [1:5];
prob = [0.6, 0.1, 0.1, 0.1, 0.1];
sig_len = 100;
sig_seq = randsrc(1, sig_len, [symbols; prob]);

%% huffman dictionary
[dict, avglen] = huffmandict(symbols, prob);

%% huffman coding
tic
hf_encode = huffmanenco(sig_seq, dict);
sz = size(hf_encode, 2);
disp(sprintf('Huffman encoding finshed!'));
disp(sprintf('Code length: %d \r\n ----------------',sz));
disp(sprintf('Encoding time: %6.4f s', toc));

%% huffman decoding
tic
sig_dec = huffmandeco(hf_encode,dict);
disp(sprintf('Huffman decoding Finshed'));
disp(sprintf('Decoding time: %6.4f s', toc));
if isequal(sig_seq,sig_dec)
    disp('Huffman lossy compression success!')
else
    disp('Huffman lossy compression failed!')
end

%% Performance discussion
H = sum(-prob.* log2(prob));% compute the entropy
disp(sprintf('Entropy:%0.6f; Dict averge length:%0.6f; Average code length:%0.6f', H, avglen, sz/sig_len));

